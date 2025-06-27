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

// JWT��Կ���ã��˴���д��
#define JWT_SECRET_KEY "whx12345678987654321"
#define CODE_EXPIRE_TIME 300 // 5����
#define CODE_LENGTH 6		 // ��֤�볤��

LoginUpService::LoginUpService() {
    initRedisClient();
}

bool LoginUpService::sendCode(const SendCodeDTO::Wrapper& dto) {
    try {
        // ������֤��
        std::string code = generateCode(CODE_LENGTH);

        // ��֤��洢��redis
        if (!storeCodeToRedis(dto->phone, code, dto->codeType, CODE_EXPIRE_TIME)) {
            return false;
        }

        // ������֤�� - ��ʱû��ǩ�����Ȳ��������
        //if (!sendSmsCode(dto->phone, code, dto->codeType)) {
        //    return false;
        //}

        LoginUpDAO dao;

        // ��֤�Ƿ��м�¼
        auto res = dao.selectLatestSmsCodeByPhone(dto->phone, dto->codeType);

        // �洢��¼
        SmsCodeDO smsCode;
        smsCode.setPhone(dto->phone);
        smsCode.setCode(code);
        smsCode.setCodeType(dto->codeType);
        smsCode.setSendTime(getCurrentTimeString());

        // �����ڼ�¼
        if (!res) {
            std::cout << "no record, insert new record\n";
            smsCode.setAttemptCount(0);
        }
        else {
            // ����洢����ĳ��Դ���
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
        // ��֤��֤��
        if (!validateCodeFromRedis(dto->phone, dto->code, "REGISTER")) {
            return nullptr;
        }

        // ����û��Ƿ��Ѵ���
        if (isUserExists(dto->phone)) {
            std::cout << "User already exists" << '\n';
            return nullptr;
        }

        // �����û�����
        UserDO user;
        user.setUserId(generateUserId());
        user.setPhone(dto->phone);
        user.setPassword(encryptPassword(dto->password));
        user.setNickname(dto->nickname ? dto->nickname : "�û�" + user.getUserId().substr(0, 8));
        user.setAvatar("http://chulan.xin/imgs/index.ico");
        user.setStatus("ACTIVE");
        user.setCreateTime(getCurrentTimeString());
        user.setUpdateTime(getCurrentTimeString());

        // �����û������ݿ�
        LoginUpDAO dao;
        int result = dao.insertUser(user);
        if (result <= 0) {
            return nullptr;
        }

        // ���ɵ�¼ƾ֤
        std::string accessToken = generateJwtToken(user.getUserId(), user.getPhone());
        std::string refreshToken = generateRefreshToken(user.getUserId());

        // ��װ���ؽ��
        loginVO->userId = user.getUserId();
        loginVO->accessToken = accessToken;
        loginVO->refreshToken = refreshToken;
        loginVO->expiresIn = 3600; // 1Сʱ

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
        // ��֤��֤��
        if (!validateCodeFromRedis(dto->phone, dto->code, "LOGIN")) {
            return nullptr;
        }

        // �����û�
        LoginUpDAO dao;
        auto user = dao.selectUserByPhone(dto->phone);
        if (!user || user->getStatus() != "ACTIVE") {
            return nullptr;
        }

        // ���ɵ�¼ƾ֤
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
        // �����û�
        LoginUpDAO dao;
        auto user = dao.selectUserByPhone(dto->phone);
        if (!user || user->getStatus() != "ACTIVE") {
            return nullptr;
        }

        // ��֤����
        if (!validatePassword(dto->password, user->getPassword())) {
            return nullptr;
        }

        // ���ɵ�¼ƾ֤
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
        // ��token����Redis������
        std::string tokenKey = "blacklist:token:" + payload.getToken();

        // ʹ��RedisClient��execute����
        return redisClient->execute<bool>([&tokenKey](Redis* redis) {
            redis->setex(tokenKey, 3600, "1"); // 1Сʱ����
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
        // ��֤��֤��
        if (!validateCodeFromRedis(dto->phone, dto->code, "RESET_PASSWORD")) {
            return false;
        }

        // �����û�
        LoginUpDAO dao;
        auto user = dao.selectUserByPhone(dto->phone);
        if (!user) {
            return false;
        }

        // ��������
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
        // ��JWT�л�ȡ�û�ID
        std::string userId = payload.getId();

        // �����û�
        LoginUpDAO dao;
        auto user = dao.selectUserById(userId);
        if (!user) {
            return nullptr;
        }

        // ��װ���ؽ��
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
        // ��֤ˢ��token
        PayloadDTO payload = verifyJwtToken(dto->refreshToken);
        if (payload.getId().empty()) {
            return nullptr;
        }

        // �����µķ���token��ˢ��token
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

// ==================== ˽�з���ʵ�� ====================

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

        // ��֤�ɹ���ɾ����֤��
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
        // ����������ģ�ⷢ�ͳɹ�
        std::cout << "ģ�ⷢ����֤�뵽�ֻ�: " << phone << ", ��֤��: " << code << ", ����: " << codeType << std::endl;
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
        // ֱ�Ӷ�ȡYAML�����ļ�
        YAML::Node config = YAML::LoadFile("conf/data-source.yaml");

        std::string host = config["spring"]["redis"]["host"].as<std::string>();
        int port = config["spring"]["redis"]["port"].as<int>();
        std::string password = config["spring"]["redis"]["password"].as<std::string>();

        redisClient = std::make_shared<RedisClient>(host, port, password);
    }
    catch (const std::exception& e) {
        std::cerr << "Init redis client error: " << e.what() << std::endl;
        // ʹ��Ĭ������
        redisClient = std::make_shared<RedisClient>("localhost", 6379, "whx051021");
    }
}

std::string LoginUpService::buildRedisKey(const std::string& phone, const std::string& codeType) {
    return "sms:code:" + codeType + ":" + phone;
}