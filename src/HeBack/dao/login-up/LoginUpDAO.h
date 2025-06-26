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

#ifndef LOGINUPDAO_H
#define LOGINUPDAO_H

#include <BaseDAO.h>
#include <domain/do/login-up/LoginUpDO.h>
#include <domain/query/login-up/LoginUpQuery.h>

/**
 * 登录注册数据库访问实现
 */
class LoginUpDAO : public BaseDAO {
public:
	// =============== 验证码相关操作 (MySQL) ===============

	// 插入验证码发送记录
	int insertSmsCodeRecord(const SmsCodeDO& smsCode);

	// 分页查询验证码发送记录
	std::list<SmsCodeDO> selectSmsCodeWithPage(const SmsCodeQuery::Wrapper& query);

	// 统计验证码发送记录数
	uint64_t countSmsCodeRecord(const SmsCodeQuery::Wrapper& query);

	// 查询某手机号最近的发送记录(用于频率限制检查)
	PtrSmsCodeDO selectLatestSmsCodeByPhone(const string& phone, const string& codeType);

	// 清理过期的验证码记录(定时任务用)
	int deleteExpiredSmsCodeRecord(int daysAgo = 30);

	// =============== 用户相关操作（MySQL） ===============

	// 插入用户
	int insertUser(const UserDO& user);

	// 通过手机号查询用户
	PtrUserDO selectUserByPhone(const string& phone);

	// 通过用户ID查询用户
	PtrUserDO selectUserById(const string& userId);

	// 更新用户基本信息
	int updateUserInfo(const UserDO& user);

	// 更新用户密码
	int updateUserPassword(const string& userId, const string& password);

	// 更新用户状态
	int updateUserStatus(const string& userId, const string& status);

	// 分页查询用户
	std::list<UserDO> selectUserWithPage(const UserQuery::Wrapper& query);

	// 统计用户数
	uint64_t countUser(const UserQuery::Wrapper& query);

private:
	// 构建验证码查询条件
	std::string buildSmsCodeQueryCondition(const SmsCodeQuery::Wrapper& query, SqlParams& params);

	// 构建用户查询条件
	std::string buildUserQueryCondition(const UserQuery::Wrapper& query, SqlParams& params);
};



#endif // !LOGINUPDAO_H
