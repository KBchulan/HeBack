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

#include <stdafx.h>
#include <sstream>

#include "LoginUpDAO.h"
#include "LoginUpMapper.h"

// =============== 验证码相关操作（MySQL - 仅用于历史记录） ===============

int LoginUpDAO::insertSmsCodeRecord(const SmsCodeDO& smsCode) {
	string sql = "INSERT INTO `t_sms_code_record` (`phone`, `code`, `code_type`, `send_time`, `attempt_count`, `ip_address`) VALUES (?, ?, ?, ?, ?, ?)";
	return sqlSession->executeUpdate(sql, "%s%s%s%s%i%s",
		smsCode.getPhone(),
		smsCode.getCode(),
		smsCode.getCodeType(),
		smsCode.getSendTime(),
		smsCode.getAttemptCount(),
		smsCode.getIpAddress());
}

std::list<SmsCodeDO> LoginUpDAO::selectSmsCodeWithPage(const SmsCodeQuery::Wrapper& query) {
	SqlParams params;
	string sql = "SELECT phone, code, code_type, send_time, attempt_count, ip_address FROM t_sms_code_record ";

	// 构建查询条件
	sql += buildSmsCodeQueryCondition(query, params);

	// 添加排序条件
	sql += " ORDER BY send_time DESC ";

	// 添加分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	SmsCodeMapper mapper;
	return sqlSession->executeQuery<SmsCodeDO>(sql, mapper, params);
}

uint64_t LoginUpDAO::countSmsCodeRecord(const SmsCodeQuery::Wrapper& query) {
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM t_sms_code_record ";

	// 构建查询条件
	sql += buildSmsCodeQueryCondition(query, params);

	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

PtrSmsCodeDO LoginUpDAO::selectLatestSmsCodeByPhone(const string& phone, const string& codeType) {
	string sql = "SELECT phone, code, code_type, send_time, attempt_count, ip_address FROM t_sms_code_record WHERE `phone`=? AND `code_type`=? ORDER BY send_time DESC LIMIT 1";
	PtrSmsCodeMapper mapper;
	return sqlSession->executeQueryOne<PtrSmsCodeDO>(sql, mapper, "%s%s", phone, codeType);
}

int LoginUpDAO::deleteExpiredSmsCodeRecord(int daysAgo) {
	string sql = "DELETE FROM `t_sms_code_record` WHERE `send_time` < DATE_SUB(NOW(), INTERVAL ? DAY)";
	return sqlSession->executeUpdate(sql, "%i", daysAgo);
}

// =============== 用户相关操作（MySQL - 持久化存储） ===============

int LoginUpDAO::insertUser(const UserDO& user) {
	string sql = "INSERT INTO `t_user` (`user_id`, `phone`, `password`, `nickname`, `avatar`, `status`, `create_time`, `update_time`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
	return sqlSession->executeUpdate(sql, "%s%s%s%s%s%s%s%s",
		user.getUserId(),
		user.getPhone(),
		user.getPassword(),
		user.getNickname(),
		user.getAvatar(),
		user.getStatus(),
		user.getCreateTime(),
		user.getUpdateTime());
}

PtrUserDO LoginUpDAO::selectUserByPhone(const string& phone) {
	string sql = "SELECT user_id, phone, password, nickname, avatar, status, create_time, update_time FROM t_user WHERE `phone`=? AND `status`!='DELETED'";
	PtrUserMapper mapper;
	return sqlSession->executeQueryOne<PtrUserDO>(sql, mapper, "%s", phone);
}

PtrUserDO LoginUpDAO::selectUserById(const string& userId) {
	string sql = "SELECT user_id, phone, password, nickname, avatar, status, create_time, update_time FROM t_user WHERE `user_id`=? AND `status`!='DELETED'";
	PtrUserMapper mapper;
	return sqlSession->executeQueryOne<PtrUserDO>(sql, mapper, "%s", userId);
}

int LoginUpDAO::updateUserInfo(const UserDO& user) {
	string sql = "UPDATE `t_user` SET `password`=?, `nickname`=?, `avatar`=?, `status`=?, `update_time`=? WHERE `user_id`=?";
	return sqlSession->executeUpdate(sql, "%s%s%s%s%s%s",
		user.getPassword(),
		user.getNickname(),
		user.getAvatar(),
		user.getStatus(),
		user.getUpdateTime(),
		user.getUserId());
}

int LoginUpDAO::updateUserPassword(const string& userId, const string& password) {
	string sql = "UPDATE `t_user` SET `password`=?, `update_time`=NOW() WHERE `user_id`=?";
	return sqlSession->executeUpdate(sql, "%s%s", password, userId);
}

int LoginUpDAO::updateUserStatus(const string& userId, const string& status) {
	string sql = "UPDATE `t_user` SET `status`=?, `update_time`=NOW() WHERE `user_id`=?";
	return sqlSession->executeUpdate(sql, "%s%s", status, userId);
}

std::list<UserDO> LoginUpDAO::selectUserWithPage(const UserQuery::Wrapper& query) {
	SqlParams params;
	string sql = "SELECT user_id, phone, password, nickname, avatar, status, create_time, update_time FROM t_user ";

	// 构建查询条件
	sql += buildUserQueryCondition(query, params);

	// 添加排序条件
	sql += " ORDER BY create_time DESC ";

	// 添加分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	UserMapper mapper;
	return sqlSession->executeQuery<UserDO>(sql, mapper, params);
}

uint64_t LoginUpDAO::countUser(const UserQuery::Wrapper& query) {
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM t_user ";

	// 构建查询条件
	sql += buildUserQueryCondition(query, params);

	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

// =============== 私有方法实现 ===============

std::string LoginUpDAO::buildSmsCodeQueryCondition(const SmsCodeQuery::Wrapper& query, SqlParams& params) {
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";

	if (query->phone && !query->phone->empty()) {
		sqlCondition << " AND `phone`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->phone.getValue(""));
	}

	if (query->codeType && !query->codeType->empty()) {
		sqlCondition << " AND `code_type`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->codeType.getValue(""));
	}

	if (query->ipAddress && !query->ipAddress->empty()) {
		sqlCondition << " AND `ip_address`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->ipAddress.getValue(""));
	}

	return sqlCondition.str();
}

std::string LoginUpDAO::buildUserQueryCondition(const UserQuery::Wrapper& query, SqlParams& params) {
	stringstream sqlCondition;
	sqlCondition << " WHERE `status`!='DELETED'";

	if (query->userId && !query->userId->empty()) {
		sqlCondition << " AND `user_id`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->userId.getValue(""));
	}

	if (query->phone && !query->phone->empty()) {
		sqlCondition << " AND `phone`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->phone.getValue(""));
	}

	if (query->nickname && !query->nickname->empty()) {
		sqlCondition << " AND `nickname` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->nickname.getValue(""));
	}

	if (query->status && !query->status->empty()) {
		sqlCondition << " AND `status`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->status.getValue(""));
	}

	return sqlCondition.str();
}