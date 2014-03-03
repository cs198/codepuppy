require 'test_helper'

class SubmissionFilesControllerTest < ActionController::TestCase
  setup do
    @submission_file = submission_files(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:submission_files)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create submission_file" do
    assert_difference('SubmissionFile.count') do
      post :create, submission_file: {  }
    end

    assert_redirected_to submission_file_path(assigns(:submission_file))
  end

  test "should show submission_file" do
    get :show, id: @submission_file
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @submission_file
    assert_response :success
  end

  test "should update submission_file" do
    patch :update, id: @submission_file, submission_file: {  }
    assert_redirected_to submission_file_path(assigns(:submission_file))
  end

  test "should destroy submission_file" do
    assert_difference('SubmissionFile.count', -1) do
      delete :destroy, id: @submission_file
    end

    assert_redirected_to submission_files_path
  end
end
