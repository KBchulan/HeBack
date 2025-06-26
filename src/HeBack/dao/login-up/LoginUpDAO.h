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
 * ��¼ע�����ݿ����ʵ��
 */
class LoginUpDAO : public BaseDAO {
public:
	// =============== ��֤����ز��� (MySQL) ===============

	// ������֤�뷢�ͼ�¼
	int insertSmsCodeRecord(const SmsCodeDO& smsCode);

	// ��ҳ��ѯ��֤�뷢�ͼ�¼
	std::list<SmsCodeDO> selectSmsCodeWithPage(const SmsCodeQuery::Wrapper& query);

	// ͳ����֤�뷢�ͼ�¼��
	uint64_t countSmsCodeRecord(const SmsCodeQuery::Wrapper& query);

	// ��ѯĳ�ֻ�������ķ��ͼ�¼(����Ƶ�����Ƽ��)
	PtrSmsCodeDO selectLatestSmsCodeByPhone(const string& phone, const string& codeType);

	// ������ڵ���֤���¼(��ʱ������)
	int deleteExpiredSmsCodeRecord(int daysAgo = 30);

	// =============== �û���ز�����MySQL�� ===============

	// �����û�
	int insertUser(const UserDO& user);

	// ͨ���ֻ��Ų�ѯ�û�
	PtrUserDO selectUserByPhone(const string& phone);

	// ͨ���û�ID��ѯ�û�
	PtrUserDO selectUserById(const string& userId);

	// �����û�������Ϣ
	int updateUserInfo(const UserDO& user);

	// �����û�����
	int updateUserPassword(const string& userId, const string& password);

	// �����û�״̬
	int updateUserStatus(const string& userId, const string& status);

	// ��ҳ��ѯ�û�
	std::list<UserDO> selectUserWithPage(const UserQuery::Wrapper& query);

	// ͳ���û���
	uint64_t countUser(const UserQuery::Wrapper& query);

private:
	// ������֤���ѯ����
	std::string buildSmsCodeQueryCondition(const SmsCodeQuery::Wrapper& query, SqlParams& params);

	// �����û���ѯ����
	std::string buildUserQueryCondition(const UserQuery::Wrapper& query, SqlParams& params);
};



#endif // !LOGINUPDAO_H
