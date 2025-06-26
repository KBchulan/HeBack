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

#ifndef LOGINUPDO_H
#define LOGINUPDO_H

#include <domain/do/DoInclude.h>

/**
 * 验证码记录数据对象
 */
class SmsCodeDO {
	// 手机号码
	CC_SYNTHESIZE(string, phone, Phone);

	// 验证码
	CC_SYNTHESIZE(string, code, Code);

	// 验证码类型（LOGIN-登录，REGISTER-注册，RESET_PASSWORD-重置密码）
	CC_SYNTHESIZE(string, codeType, CodeType);
	
	// 发送时间
	CC_SYNTHESIZE(string, sendTime, SendTime);
	
	// 尝试次数
	CC_SYNTHESIZE(int, attemptCount, AttemptCount);
	
	// IP地址
	CC_SYNTHESIZE(string, ipAddress, IpAddress);

public:
	SmsCodeDO() {
		phone = "";
		code = "";
		codeType = "LOGIN";
		sendTime = "";
		attemptCount = 0;
		ipAddress = "";
	}
};

/**
 * 用户信息数据对象
 */
class UserDO {
	// 用户ID
	CC_SYNTHESIZE(string, userId, UserId);

	// 手机号码
	CC_SYNTHESIZE(string, phone, Phone);
	
	// 密码 - 需加密
	CC_SYNTHESIZE(string, password, Password);

	// 用户昵称
	CC_SYNTHESIZE(string, nickname, Nickname);
	
	// 头像URL
	CC_SYNTHESIZE(string, avatar, Avatar);
	
	// 用户状态（ACTIVE-激活，INACTIVE-未激活，LOCKED-锁定）
	CC_SYNTHESIZE(string, status, Status);
	
	// 创建时间
	CC_SYNTHESIZE(string, createTime, CreateTime);
	
	// 更新时间
	CC_SYNTHESIZE(string, updateTime, UpdateTime);

public:
	UserDO() {
		userId = "";
		phone = "";
		password = "";
		nickname = "";
		avatar = "";
		status = "ACTIVE";
		createTime = "";
		updateTime = "";
	}
};

// 智能指针类型定义
typedef std::shared_ptr<SmsCodeDO> PtrSmsCodeDO;
typedef std::shared_ptr<UserDO> PtrUserDO;

#endif // !LOGINUPDO_H
