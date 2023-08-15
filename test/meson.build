gtest_dep = dependency('gtest') 
gtest_main_dep = dependency('gtest_main')

#create a test executable for each test suite
topic_test = executable('vespula', 'topic_test.cpp', dependencies
               :[ communication_dep, gtest_dep, gtest_main_dep ], install
               : true)

test('TopicTest', topic_test)
