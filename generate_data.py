import json
import random


class Person:
    """id，喜好集，关注集，粉丝集"""

    def __init__(self, _id, name):
        self._id = _id
        self.name = name
        self.hobbies = set()
        self.watches = set()
        self.fans = set()

    def add_hobby(self, hobby):
        self.hobbies.add(hobby)

    def remove_hobby(self, hobby):
        if hobby in self.hobbies:
            self.hobbies.remove(hobby)

    def get_bobbies(self):
        return self.hobbies

    def add_watch(self, watch):
        if watch != self._id:
            self.watches.add(watch)

    def remove_watch(self, watch):
        if watch in self.watches:
            self.watches.remove(watch)

    def get_watches(self):
        return self.watches

    def add_fan(self, fan):
        if fan != self._id:
            self.fans.add(fan)

    def remove_fan(self, fan):
        if fan in self.fans:
            self.fans.remove(fan)

    def get_fans(self):
        return self.fans

    def __repr__(self):
        return 'id: ' + str(self._id) + ', likes: ' + str(self.hobbies) \
               + ', watches: ' + str(self.watches) + ', fans: ' \
               + str(self.fans)


with open('hobbies.json', 'r') as fp:
    hobbies = [hobby['hobby'] for hobby in json.load(fp)]
with open('name.json', 'r') as fp:
    names = [name['name'] for name in json.load(fp)]
# 先将每个人的关注集生成，根据关注集往对应人里面添加粉丝
people = {_id: Person(_id, random.choice(names)) for _id in range(2000)}
# 暂且将喜好范围设置，一个人多次添加机会
# 关注的范围就是所有人了，一个人多次添加机会
population = len(people)

for person in people.values():
    # 添加爱好
    for i in range(20):
        person.add_hobby(random.choice(hobbies))
    # 添加关注的人
    for i in range(50):
        person.add_watch(random.choice(range(population)))

# 开始处理粉丝问题
for _id, person in people.items():
    for watch in person.get_watches():
        people[watch].add_fan(_id)
# 写入文件
with open('data.txt', 'w') as fp:
    fp.write(str(population) + '\n\n')
    for _id, person in people.items():
        # 写入id
        fp.write(str(_id) + '\n')
        # 写入喜好，-1结尾
        fp.write(person.name + '\n')
        for hobby in person.get_bobbies():
            fp.write(str(hobby) + ' ')
        # -1结尾并换行
        fp.write('-1\n')
        # 写入关注
        for watch in person.get_watches():
            fp.write(str(watch) + ' ')
        # -1换行
        fp.write('-1\n')
        for fan in person.get_fans():
            fp.write(str(fan) + ' ')
        # -1换行
        fp.write('-1\n\n')
        # 写入结束
print('save to file done.')
