#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: Chulan
 @Date: 2025/06/26 13:24:32

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

#ifndef LOGINUPVO_H
#define LOGINUPVO_H

#include <domain/GlobalInclude.h>
#include <domain/dto/login-up/LoginUpDTO.h>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * ��¼��ӦVO - ����token��Ϣ
 */
class LoginVO : public oatpp::DTO {
	DTO_INIT(LoginVO, DTO);

	// �û�ID
	DTO_FIELD(String, userId);
	DTO_FIELD_INFO(userId) {
		info->description = ZH_WORDS_GETTER("loginup.field.userId");
	}

	// ��������
	DTO_FIELD(String, accessToken);
	DTO_FIELD_INFO(accessToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.accessToken");
	}

	// ˢ������
	DTO_FIELD(String, refreshToken);
	DTO_FIELD_INFO(refreshToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.refreshToken");
	}

	// ���ƹ���ʱ�䣨�룩
	DTO_FIELD(Int64, expiresIn);
	DTO_FIELD_INFO(expiresIn) {
		info->description = ZH_WORDS_GETTER("loginup.field.expiresIn");
	}
};

/**
 * �û���ϢVO
 */
class UserProfileVO : public oatpp::DTO {
	DTO_INIT(UserProfileVO, DTO);

	// �û�ID
	DTO_FIELD(String, userId);
	DTO_FIELD_INFO(userId) {
		info->description = ZH_WORDS_GETTER("loginup.field.userId");
	}

	// �ֻ�����
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// �û��ǳ�
	DTO_FIELD(String, nickname);
	DTO_FIELD_INFO(nickname) {
		info->description = ZH_WORDS_GETTER("loginup.field.nickname");
	}

	// ע��ʱ��
	DTO_FIELD(String, registerTime);
	DTO_FIELD_INFO(registerTime) {
		info->description = ZH_WORDS_GETTER("loginup.field.registerTime");
	}
};

/**
 * ˢ��Token��ӦVO
 */
class RefreshTokenVO : public oatpp::DTO {
	DTO_INIT(RefreshTokenVO, DTO);

	// �µķ�������
	DTO_FIELD(String, accessToken);
	DTO_FIELD_INFO(accessToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.accessToken");
	}

	// �µ�ˢ������
	DTO_FIELD(String, refreshToken);
	DTO_FIELD_INFO(refreshToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.refreshToken");
	}

	// ���ƹ���ʱ�䣨�룩
	DTO_FIELD(Int64, expiresIn);
	DTO_FIELD_INFO(expiresIn) {
		info->description = ZH_WORDS_GETTER("loginup.field.expiresIn");
	}
};

// JSON��װ������
using LoginJsonVO = JsonVO<LoginVO::Wrapper>;
using UserProfileJsonVO = JsonVO<UserProfileVO::Wrapper>;
using RefreshTokenJsonVO = JsonVO<RefreshTokenVO::Wrapper>;

#include OATPP_CODEGEN_END(DTO)

#endif // !LOGINUPVO_H
