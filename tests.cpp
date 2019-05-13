#include "testing_core.hxx"

TEST(is_model_correct, is_model_correct){
    bool mustBeTrue = is_model_correct();
    ASSERT_TRUE(mustBeTrue);
}
TEST(is_view_correct, is_view_correct){
    bool mustBeTrue = is_view_correct();
    ASSERT_TRUE(mustBeTrue);
}
TEST(is_controller_correct, is_controller_correct){
    bool mustBeTrue = is_controller_correct();
    ASSERT_TRUE(mustBeTrue);
}
