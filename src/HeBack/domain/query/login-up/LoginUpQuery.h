#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: Chulan
 @Date: 2025/06/26 13:17:32

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

#ifndef LOGINUP_QUERY_H
#define LOGINUP_QUERY_H

#include <domain/GlobalInclude.h>
#include <domain/query/PageQuery.h>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 验证码记录查询对象
 */
class SmsCodeQuery : public PageQuery {
	DTO_INIT(SmsCodeQuery, PageQuery);

	// 手机号码
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// 验证码类型
	DTO_FIELD(String, codeType);
	DTO_FIELD_INFO(codeType) {
		info->description = ZH_WORDS_GETTER("loginup.field.codeType");
	}
	
	// IP地址
	DTO_FIELD(String, ipAddress);
	DTO_FIELD_INFO(ipAddress) {
		info->description = ZH_WORDS_GETTER("loginup.field.ipAddress");
	}
};

/**
 * 用户信息查询对象
 */
class UserQuery : public PageQuery {
	DTO_INIT(UserQuery, PageQuery);

	// 用户ID
	DTO_FIELD(String, userId);
	DTO_FIELD_INFO(userId) {
		info->description = ZH_WORDS_GETTER("loginup.field.userId");
	}

	// 手机号码
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// 用户昵称
	DTO_FIELD(String, nickname);
	DTO_FIELD_INFO(nickname) {
		info->description = ZH_WORDS_GETTER("loginup.field.nickname");
	}

	// 用户状态
	DTO_FIELD(String, status);
	DTO_FIELD_INFO(status) {
		info->description = ZH_WORDS_GETTER("loginup.field.status");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !LOGINUP_QUERY_H
