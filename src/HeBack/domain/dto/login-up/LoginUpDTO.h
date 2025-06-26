#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: Chulan
 @Date: 2025/06/26 13:09:32

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

#ifndef LOGINUP_DTO_H
#define LOGINUP_DTO_H

#include <domain/GlobalInclude.h>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 1. ������֤������DTO
 */
class SendCodeDTO : public oatpp::DTO {
	DTO_INIT(SendCodeDTO, DTO);

	// �ֻ�����
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// ��֤������
	DTO_FIELD(String, codeType);
	DTO_FIELD_INFO(codeType) {
		info->description = ZH_WORDS_GETTER("loginup.field.codeType");
	}
};

/**
 * 2. �û�ע��DTO
 */
class RegisterDTO : public oatpp::DTO {
	DTO_INIT(RegisterDTO, DTO);

	// �ֻ�����
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// ��֤��
	DTO_FIELD(String, code);
	DTO_FIELD_INFO(code) {
		info->description = ZH_WORDS_GETTER("loginup.field.code");
	}

	// ����
	DTO_FIELD(String, password);
	DTO_FIELD_INFO(password) {
		info->description = ZH_WORDS_GETTER("loginup.field.password");
	}

	// �û��ǳƣ���ѡ��
	DTO_FIELD(String, nickname);
	DTO_FIELD_INFO(nickname) {
		info->description = ZH_WORDS_GETTER("loginup.field.nickname");
	}
};

/**
 * 3.1 �û���¼DTO - ��֤���¼
 */
class LoginByCodeDTO : public oatpp::DTO {
	DTO_INIT(LoginByCodeDTO, DTO);

	// �ֻ�����
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// ��֤��
	DTO_FIELD(String, code);
	DTO_FIELD_INFO(code) {
		info->description = ZH_WORDS_GETTER("loginup.field.code");
	}
};

/**
 * 3.2 �û���¼DTO - �����¼
 */
class LoginByPasswordDTO : public oatpp::DTO {
	DTO_INIT(LoginByPasswordDTO, DTO);

	// �ֻ�����
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// ����
	DTO_FIELD(String, password);
	DTO_FIELD_INFO(password) {
		info->description = ZH_WORDS_GETTER("loginup.field.password");
	}
};

/**
 * 4 �û��ǳ�DTO
 */
class LogoutDTO : public oatpp::DTO {
	DTO_INIT(LogoutDTO, DTO);

	// ��������
	DTO_FIELD(String, accessToken);
	DTO_FIELD_INFO(accessToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.accessToken");
	}
};

/**
 * 5 ��������DTO
 */
class ResetPasswordDTO : public oatpp::DTO {
	DTO_INIT(ResetPasswordDTO, DTO);

	// �ֻ�����
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// ��֤��
	DTO_FIELD(String, code);
	DTO_FIELD_INFO(code) {
		info->description = ZH_WORDS_GETTER("loginup.field.code");
	}

	// ������
	DTO_FIELD(String, newPassword);
	DTO_FIELD_INFO(newPassword) {
		info->description = ZH_WORDS_GETTER("loginup.field.newPassword");
	}
};

/**
 * 6 ˢ��Token DTO
 */
class RefreshTokenDTO : public oatpp::DTO {
	DTO_INIT(RefreshTokenDTO, DTO);

	// ˢ������
	DTO_FIELD(String, refreshToken);
	DTO_FIELD_INFO(refreshToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.refreshToken");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !LOGINUP_DTO_H
