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
 * ��֤���¼���ݶ���
 */
class SmsCodeDO {
	// �ֻ�����
	CC_SYNTHESIZE(string, phone, Phone);

	// ��֤��
	CC_SYNTHESIZE(string, code, Code);

	// ��֤�����ͣ�LOGIN-��¼��REGISTER-ע�ᣬRESET_PASSWORD-�������룩
	CC_SYNTHESIZE(string, codeType, CodeType);
	
	// ����ʱ��
	CC_SYNTHESIZE(string, sendTime, SendTime);
	
	// ���Դ���
	CC_SYNTHESIZE(int, attemptCount, AttemptCount);
	
	// IP��ַ
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
 * �û���Ϣ���ݶ���
 */
class UserDO {
	// �û�ID
	CC_SYNTHESIZE(string, userId, UserId);

	// �ֻ�����
	CC_SYNTHESIZE(string, phone, Phone);
	
	// ���� - �����
	CC_SYNTHESIZE(string, password, Password);

	// �û��ǳ�
	CC_SYNTHESIZE(string, nickname, Nickname);
	
	// ͷ��URL
	CC_SYNTHESIZE(string, avatar, Avatar);
	
	// �û�״̬��ACTIVE-���INACTIVE-δ���LOCKED-������
	CC_SYNTHESIZE(string, status, Status);
	
	// ����ʱ��
	CC_SYNTHESIZE(string, createTime, CreateTime);
	
	// ����ʱ��
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

// ����ָ�����Ͷ���
typedef std::shared_ptr<SmsCodeDO> PtrSmsCodeDO;
typedef std::shared_ptr<UserDO> PtrUserDO;

#endif // !LOGINUPDO_H
