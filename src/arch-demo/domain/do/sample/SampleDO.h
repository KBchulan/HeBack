#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:52:32

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
#ifndef _SAMPLE_DO_
#define _SAMPLE_DO_
#include "../DoInclude.h"

/**
 * 示例数据库实体类
 */
class SampleDO
{
	// 编号
	CC_SYNTHESIZE(string, id, Id);
	// 姓名
	CC_SYNTHESIZE(string, name, Name);
	// 性别
	CC_SYNTHESIZE(string, sex, Sex);
	// 年龄
	CC_SYNTHESIZE(int, age, Age);
	// 创建人
	CC_SYNTHESIZE(string, createBy, CreateBy);
	// 创建时间
	CC_SYNTHESIZE(string, createTime, CreateTime);
    // 修改人
    CC_SYNTHESIZE(string, updateBy, UpdateBy);
    // 修改时间
    CC_SYNTHESIZE(string, updateTime, UpdateTime);
public:
	SampleDO() {
		id = "";
		name = "";
		sex = "";
		age = -1;
	}
};

// 给SampleDO智能指针设定一个别名方便使用
typedef std::shared_ptr<SampleDO> PtrSampleDO;
#endif // !_SAMPLE_DO_
