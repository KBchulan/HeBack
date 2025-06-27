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
 * ��¼ע��ҵ���߼���
 */
class LoginUpService {
public:
	// ���캯��
	LoginUpService();

	// ������֤��
	bool sendCode(const SendCodeDTO::Wrapper& dto);

	// �û�ע��
	LoginVO::Wrapper registerUser(const RegisterDTO::Wrapper& dto);

	// �û���¼ (��֤���¼)
	LoginVO::Wrapper loginByCode(const LoginByCodeDTO::Wrapper& dto);

	// �û���¼ (�����¼)
	LoginVO::Wrapper loginByPassword(const LoginByPasswordDTO::Wrapper& dto);

	// �û��ǳ�
	bool logout(const PayloadDTO& payload);

	// ��������
	bool resetPassword(const ResetPasswordDTO::Wrapper& dto);

	// ��ȡ�û���Ϣ
	UserProfileVO::Wrapper getUserProfile(const PayloadDTO& payload);

	// ˢ��Token
	RefreshTokenVO::Wrapper refreshToken(const RefreshTokenDTO::Wrapper& dto);

private:
	// Redis�ͻ���
	std::shared_ptr<RedisClient> redisClient;

	// �����û�ID
	std::string generateUserId();

	// ����JWT Token
	std::string generateJwtToken(const std::string& userId, const std::string& phone, int64_t expiresIn = 3600); // 1Сʱ

	// ����ˢ��Token
	std::string generateRefreshToken(const std::string& userId, int64_t expiresIn = 86400 * 7); // 7��

	// ��֤JWT Token
	PayloadDTO verifyJwtToken(const std::string& token);

	// ��������
	std::string encryptPassword(const std::string& password);

	// ��֤����
	bool validatePassword(const std::string& password, const std::string& hashedPassword);

	// ������֤��
	std::string generateCode(int length = 6);

	// �洢��֤�뵽Redis
	bool storeCodeToRedis(const std::string& phone, const std::string& code, const std::string& codeType, int expireSeconds = 300);

	// ��Redis��֤��֤��
	bool validateCodeFromRedis(const std::string& phone, const std::string& code, const std::string& codeType);

	// ���Ͷ�����֤��
	bool sendSmsCode(const std::string& phone, const std::string& code, const std::string& codeType);

	// ����û��Ƿ����
	bool isUserExists(const std::string& phone);

	// ��ȡ��ǰʱ���ַ���
	std::string getCurrentTimeString();

	// ��ʼ��Redis����
	void initRedisClient();

	// ����Redis Key
	std::string buildRedisKey(const std::string& phone, const std::string& codeType);
};

#endif // !LOGINUPSERVICE_H
