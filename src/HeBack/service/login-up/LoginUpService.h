#pragma once

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

#ifndef LOGINUPSERVICE_H
#define LOGINUPSERVICE_H

#include <domain/dto/login-up/LoginUpDTO.h>
#include <domain/vo/login-up/LoginUpVO.h>
#include <domain/dto/PageDTO.h>
#include <RedisClient.h>

/**
 * 登录注册业务逻辑类
 */
class LoginUpService {
public:
	// 构造函数
	LoginUpService();

	// 发送验证码
	bool sendCode(const SendCodeDTO::Wrapper& dto);

	// 用户注册
	LoginVO::Wrapper registerUser(const RegisterDTO::Wrapper& dto);

	// 用户登录 (验证码登录)
	LoginVO::Wrapper loginByCode(const LoginByCodeDTO::Wrapper& dto);

	// 用户登录 (密码登录)
	LoginVO::Wrapper loginByPassword(const LoginByPasswordDTO::Wrapper& dto);

	// 用户登出
	bool logout(const PayloadDTO& payload);

	// 重置密码
	bool resetPassword(const ResetPasswordDTO::Wrapper& dto);

	// 获取用户信息
	UserProfileVO::Wrapper getUserProfile(const PayloadDTO& payload);

	// 刷新Token
	RefreshTokenVO::Wrapper refreshToken(const RefreshTokenDTO::Wrapper& dto);

private:
	// Redis客户端
	std::shared_ptr<RedisClient> redisClient;

	// 生成用户ID
	std::string generateUserId();

	// 生成JWT Token
	std::string generateJwtToken(const std::string& userId, const std::string& phone, int64_t expiresIn = 3600); // 1小时

	// 生成刷新Token
	std::string generateRefreshToken(const std::string& userId, int64_t expiresIn = 86400 * 7); // 7天

	// 验证JWT Token
	PayloadDTO verifyJwtToken(const std::string& token);

	// 加密密码
	std::string encryptPassword(const std::string& password);

	// 验证密码
	bool validatePassword(const std::string& password, const std::string& hashedPassword);

	// 生成验证码
	std::string generateCode(int length = 6);

	// 存储验证码到Redis
	bool storeCodeToRedis(const std::string& phone, const std::string& code, const std::string& codeType, int expireSeconds = 300);

	// 从Redis验证验证码
	bool validateCodeFromRedis(const std::string& phone, const std::string& code, const std::string& codeType);

	// 发送短信验证码
	bool sendSmsCode(const std::string& phone, const std::string& code, const std::string& codeType);

	// 检查用户是否存在
	bool isUserExists(const std::string& phone);

	// 获取当前时间字符串
	std::string getCurrentTimeString();

	// 初始化Redis连接
	void initRedisClient();

	// 构建Redis Key
	std::string buildRedisKey(const std::string& phone, const std::string& codeType);
};

#endif // !LOGINUPSERVICE_H
