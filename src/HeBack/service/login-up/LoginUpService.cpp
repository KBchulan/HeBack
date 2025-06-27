/*
 Copyright Zero One Star. All rights reserved.

 @Author: Chulan
 @Date: 2025/06/24 13:04:32

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "stdafx.h"
#include "LoginUpService.h"
#include <domain/do/login-up/LoginUpDO.h>
#include <dao/login-up/LoginUpDAO.h>
#include "SimpleDateTimeFormat.h"
#include "JWTUtil.h"
#include "bcrypt/bcrypt.h"
#include "id/UuidFacade.h"
#include "YamlHelper.h"
#include <random>
#include <sstream>
#include <fstream>

#ifdef USE_ALISMS
#include "sms/aliyun/AliSmsSender.h"
#endif

// JWT密钥配置，此处先写死
#define JWT_SECRET_KEY "whx12345678987654321"
#define CODE_EXPIRE_TIME 300 // 5分钟
#define CODE_LENGTH 6		 // 验证码长度

LoginUpService::LoginUpService() {
    initRedisClient();
}

bool LoginUpService::sendCode(const SendCodeDTO::Wrapper& dto) {
    try {
        // 生成验证码
        std::string code = generateCode(CODE_LENGTH);

        // 验证码存储到redis
        if (!storeCodeToRedis(dto->phone, code, dto->codeType, CODE_EXPIRE_TIME)) {
            return false;
        }

        // 发送验证码 - 暂时没有签名，先不考虑这个
        //if (!sendSmsCode(dto->phone, code, dto->codeType)) {
        //    return false;
        //}

        LoginUpDAO dao;

        // 验证是否有记录
        auto res = dao.selectLatestSmsCodeByPhone(dto->phone, dto->codeType);

        // 存储记录
        SmsCodeDO smsCode;
        smsCode.setPhone(dto->phone);
        smsCode.setCode(code);
        smsCode.setCodeType(dto->codeType);
        smsCode.setSendTime(getCurrentTimeString());

        // 不存在记录
        if (!res) {
            std::cout << "no record, insert new record\n";
            smsCode.setAttemptCount(0);
        }
        else {
            // 否则存储更多的尝试次数
            auto newAttempt = res->getAttemptCount() + 1;
            smsCode.setAttemptCount(newAttempt);
        }
        
        smsCode.setIpAddress(dto->ipAddress);

        dao.insertSmsCodeRecord(smsCode);

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "send code error: " << e.what() << '\n';
        return false;
    }
}

LoginVO::Wrapper LoginUpService::registerUser(const RegisterDTO::Wrapper& dto) {
    auto loginVO = LoginVO::createShared();

    try {
        // 验证验证码
        if (!validateCodeFromRedis(dto->phone, dto->code, "REGISTER")) {
            return nullptr;
        }

        // 检查用户是否已存在
        if (isUserExists(dto->phone)) {
            std::cout << "User already exists" << '\n';
            return nullptr;
        }

        // 创建用户数据
        UserDO user;
        user.setUserId(generateUserId());
        user.setPhone(dto->phone);
        user.setPassword(encryptPassword(dto->password));
        user.setNickname(dto->nickname ? dto->nickname : "用户" + user.getUserId().substr(0, 8));
        user.setAvatar("http://chulan.xin/imgs/index.ico");
        user.setStatus("ACTIVE");
        user.setCreateTime(getCurrentTimeString());
        user.setUpdateTime(getCurrentTimeString());

        // 保存用户到数据库
        LoginUpDAO dao;
        int result = dao.insertUser(user);
        if (result <= 0) {
            return nullptr;
        }

        // 生成登录凭证
        std::string accessToken = generateJwtToken(user.getUserId(), user.getPhone());
        std::string refreshToken = generateRefreshToken(user.getUserId());

        // 封装返回结果
        loginVO->userId = user.getUserId();
        loginVO->accessToken = accessToken;
        loginVO->refreshToken = refreshToken;
        loginVO->expiresIn = 3600; // 1小时

        return loginVO;
    }
    catch (const std::exception& e) {
        std::cerr << "Register user error: " << e.what() << std::endl;
        return nullptr;
    }
}

LoginVO::Wrapper LoginUpService::loginByCode(const LoginByCodeDTO::Wrapper& dto) {
    auto loginVO = LoginVO::createShared();

    try {
        // 验证验证码
        if (!validateCodeFromRedis(dto->phone, dto->code, "LOGIN")) {
            return nullptr;
        }

        // 查找用户
        LoginUpDAO dao;
        auto user = dao.selectUserByPhone(dto->phone);
        if (!user || user->getStatus() != "ACTIVE") {
            return nullptr;
        }

        // 生成登录凭证
        std::string accessToken = generateJwtToken(user->getUserId(), user->getPhone());
        std::string refreshToken = generateRefreshToken(user->getUserId());

        loginVO->userId = user->getUserId();
        loginVO->accessToken = accessToken;
        loginVO->refreshToken = refreshToken;
        loginVO->expiresIn = 3600;

        return loginVO;
    }
    catch (const std::exception& e) {
        std::cerr << "Login by code error: " << e.what() << std::endl;
        return nullptr;
    }
}

LoginVO::Wrapper LoginUpService::loginByPassword(const LoginByPasswordDTO::Wrapper& dto) {
    auto loginVO = LoginVO::createShared();

    try {
        // 查找用户
        LoginUpDAO dao;
        auto user = dao.selectUserByPhone(dto->phone);
        if (!user || user->getStatus() != "ACTIVE") {
            return nullptr;
        }

        // 验证密码
        if (!validatePassword(dto->password, user->getPassword())) {
            return nullptr;
        }

        // 生成登录凭证
        std::string accessToken = generateJwtToken(user->getUserId(), user->getPhone());
        std::string refreshToken = generateRefreshToken(user->getUserId());

        std::cout << accessToken << '\n';
        std::cout << refreshToken << '\n';

        loginVO->userId = user->getUserId();
        loginVO->accessToken = accessToken;
        loginVO->refreshToken = refreshToken;
        loginVO->expiresIn = 3600;

        return loginVO;
    }
    catch (const std::exception& e) {
        std::cerr << "Login by password error: " << e.what() << std::endl;
        return nullptr;
    }
}

bool LoginUpService::logout(const PayloadDTO& payload) {
    try {
        // 将token加入Redis黑名单
        std::string tokenKey = "blacklist:token:" + payload.getToken();

        // 使用RedisClient的execute方法
        return redisClient->execute<bool>([&tokenKey](Redis* redis) {
            redis->setex(tokenKey, 3600, "1"); // 1小时过期
            return true;
            });
    }
    catch (const std::exception& e) {
        std::cerr << "Logout error: " << e.what() << '\n';
        return false;
    }
}

bool LoginUpService::resetPassword(const ResetPasswordDTO::Wrapper& dto) {
    try {
        // 验证验证码
        if (!validateCodeFromRedis(dto->phone, dto->code, "RESET_PASSWORD")) {
            return false;
        }

        // 查找用户
        LoginUpDAO dao;
        auto user = dao.selectUserByPhone(dto->phone);
        if (!user) {
            return false;
        }

        // 更新密码
        user->setPassword(encryptPassword(dto->newPassword));
        user->setUpdateTime(getCurrentTimeString());

        return dao.updateUserInfo(*user) > 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Reset password error: " << e.what() << std::endl;
        return false;
    }
}

UserProfileVO::Wrapper LoginUpService::getUserProfile(const PayloadDTO& payload) {
    auto profileVO = UserProfileVO::createShared();

    try {
        // 从JWT中获取用户ID
        std::string userId = payload.getId();

        // 查找用户
        LoginUpDAO dao;
        auto user = dao.selectUserById(userId);
        if (!user) {
            return nullptr;
        }

        // 封装返回结果
        profileVO->userId = user->getUserId();
        profileVO->phone = user->getPhone();
        profileVO->nickname = user->getNickname();
        profileVO->registerTime = user->getCreateTime();

        return profileVO;
    }
    catch (const std::exception& e) {
        std::cerr << "Get user profile error: " << e.what() << std::endl;
        return nullptr;
    }
}

RefreshTokenVO::Wrapper LoginUpService::refreshToken(const RefreshTokenDTO::Wrapper& dto) {
    auto refreshVO = RefreshTokenVO::createShared();

    try {
        // 验证刷新token
        PayloadDTO payload = verifyJwtToken(dto->refreshToken);
        if (payload.getId().empty()) {
            return nullptr;
        }

        // 生成新的访问token和刷新token
        std::string newAccessToken = generateJwtToken(payload.getId(), payload.getUsername());
        std::string newRefreshToken = generateRefreshToken(payload.getId());

        refreshVO->accessToken = newAccessToken;
        refreshVO->refreshToken = newRefreshToken;
        refreshVO->expiresIn = 3600;

        return refreshVO;
    }
    catch (const std::exception& e) {
        std::cerr << "Refresh token error: " << e.what() << std::endl;
        return nullptr;
    }
}

// ==================== 私有方法实现 ====================

std::string LoginUpService::generateUserId() {
    UuidFacade uuid;
    return uuid.genUuid();
}

std::string LoginUpService::generateJwtToken(const std::string& userId, const std::string& phone, int64_t expiresIn) {
    PayloadDTO payload;
    payload.setId(userId);
    payload.setUsername(phone);
    payload.setExp(expiresIn);

    return JWTUtil::generateTokenByHmac(payload, JWT_SECRET_KEY);
}

std::string LoginUpService::generateRefreshToken(const std::string& userId, int64_t expiresIn) {
    PayloadDTO payload;
    payload.setId(userId);
    payload.setExp(expiresIn);

    return JWTUtil::generateTokenByHmac(payload, JWT_SECRET_KEY);
}

PayloadDTO LoginUpService::verifyJwtToken(const std::string& token) {
    return JWTUtil::verifyTokenByHmac(token, JWT_SECRET_KEY);
}

std::string LoginUpService::encryptPassword(const std::string& password) {
    return bcrypt::generateHash(password);
}

bool LoginUpService::validatePassword(const std::string& password, const std::string& hashedPassword) {
    return bcrypt::validatePassword(password, hashedPassword);
}

std::string LoginUpService::generateCode(int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string code;
    for (int i = 0; i < length; ++i) {
        code += std::to_string(dis(gen));
    }
    return code;
}

bool LoginUpService::storeCodeToRedis(const std::string& phone, const std::string& code, const std::string& codeType, int expireSeconds) {
    try {
        std::string key = buildRedisKey(phone, codeType);
        return redisClient->execute<bool>([&key, &code, expireSeconds](Redis* redis) {
            redis->setex(key, expireSeconds, code);
            return true;
            });
    }
    catch (const std::exception& e) {
        std::cerr << "Store code to redis error: " << e.what() << '\n';
        return false;
    }
}

bool LoginUpService::validateCodeFromRedis(const std::string& phone, const std::string& code, const std::string& codeType) {
    try {
        std::string key = buildRedisKey(phone, codeType);
        std::string storedCode = redisClient->execute<std::string>([&key](Redis* redis) {
            return redis->get(key).value_or("");
            });

        if (storedCode.empty() || storedCode != code) {
            return false;
        }

        // 验证成功后删除验证码
        redisClient->execute<bool>([&key](Redis* redis) {
            redis->del(key);
            return true;
            });
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Validate code from redis error: " << e.what() << std::endl;
        return false;
    }
}

bool LoginUpService::sendSmsCode(const std::string& phone, const std::string& code, const std::string& codeType) {
    try {
#ifdef USE_ALISMS
        AliSmsSender sender;
        return sender.sendCode(phone, code, codeType);
#else
        // 开发环境下模拟发送成功
        std::cout << "模拟发送验证码到手机: " << phone << ", 验证码: " << code << ", 类型: " << codeType << std::endl;
        return true;
#endif
    }
    catch (const std::exception& e) {
        std::cerr << "Send sms code error: " << e.what() << std::endl;
        return false;
    }
}

bool LoginUpService::isUserExists(const std::string& phone) {
    try {
        LoginUpDAO dao;
        auto user = dao.selectUserByPhone(phone);
        return user != nullptr;
    }
    catch (const std::exception& e) {
        std::cerr << "Check user exists error: " << e.what() << std::endl;
        return false;
    }
}

std::string LoginUpService::getCurrentTimeString() {
    return SimpleDateTimeFormat::format();
}

void LoginUpService::initRedisClient() {
    try {
        // 直接读取YAML配置文件
        YAML::Node config = YAML::LoadFile("conf/data-source.yaml");

        std::string host = config["spring"]["redis"]["host"].as<std::string>();
        int port = config["spring"]["redis"]["port"].as<int>();
        std::string password = config["spring"]["redis"]["password"].as<std::string>();

        redisClient = std::make_shared<RedisClient>(host, port, password);
    }
    catch (const std::exception& e) {
        std::cerr << "Init redis client error: " << e.what() << std::endl;
        // 使用默认配置
        redisClient = std::make_shared<RedisClient>("localhost", 6379, "whx051021");
    }
}

std::string LoginUpService::buildRedisKey(const std::string& phone, const std::string& codeType) {
    return "sms:code:" + codeType + ":" + phone;
}