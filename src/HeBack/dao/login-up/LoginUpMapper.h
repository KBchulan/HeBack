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

#ifndef LOGINUPMAPPER_H
#define LOGINUPMAPPER_H

#include <Mapper.h>
#include <domain/do/login-up/LoginUpDO.h>

/*
 * 验证码记录字段映射
 */
class SmsCodeMapper : public Mapper<SmsCodeDO> {
public:
	SmsCodeDO mapper(ResultSet* resultSet) const override {
		SmsCodeDO data;
		data.setPhone(resultSet->getString(1));
		data.setCode(resultSet->getString(2));
		data.setCodeType(resultSet->getString(3));
		data.setSendTime(resultSet->getString(4));
		data.setAttemptCount(resultSet->getInt(5));
		data.setIpAddress(resultSet->getString(6));
		return data;
	}
};

/**
 * 验证码记录字段匹配映射-智能指针版本
 */
class PtrSmsCodeMapper : public Mapper<PtrSmsCodeDO> {
public:
	PtrSmsCodeDO mapper(ResultSet* resultSet) const override {
		auto data = std::make_shared<SmsCodeDO>();
		data->setPhone(resultSet->getString(1));
		data->setCode(resultSet->getString(2));
		data->setCodeType(resultSet->getString(3));
		data->setSendTime(resultSet->getString(4));
		data->setAttemptCount(resultSet->getInt(5));
		data->setIpAddress(resultSet->getString(6));
		return data;
	}
};

/**
 * 用户信息字段匹配映射
 */
class UserMapper : public Mapper<UserDO> {
public:
	UserDO mapper(ResultSet* resultSet) const override {
		UserDO data;
		data.setUserId(resultSet->getString(1));
		data.setPhone(resultSet->getString(2));
		data.setPassword(resultSet->getString(3));
		data.setNickname(resultSet->getString(4));
		data.setAvatar(resultSet->getString(5));
		data.setStatus(resultSet->getString(6));
		data.setCreateTime(resultSet->getString(7));
		data.setUpdateTime(resultSet->getString(8));
		return data;
	}
};

/**
 * 用户信息字段匹配映射-智能指针版本
 */
class PtrUserMapper : public Mapper<PtrUserDO> {
public:
	PtrUserDO mapper(ResultSet* resultSet) const override {
		auto data = std::make_shared<UserDO>();
		data->setUserId(resultSet->getString(1));
		data->setPhone(resultSet->getString(2));
		data->setPassword(resultSet->getString(3));
		data->setNickname(resultSet->getString(4));
		data->setAvatar(resultSet->getString(5));
		data->setStatus(resultSet->getString(6));
		data->setCreateTime(resultSet->getString(7));
		data->setUpdateTime(resultSet->getString(8));
		return data;
	}
};

#endif // !LOGINUPMAPPER_H
