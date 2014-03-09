require 'test_helper'

class SessionControllerTest < ActionController::TestCase
  test "should get verify" do
    get :verify
    assert_response :success
  end

end
