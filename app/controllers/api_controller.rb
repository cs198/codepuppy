class ApiController < ApplicationController
  protect_from_forgery with: :null_session
  before_filter :api_session_authenticate!

  private

  def signed_in?
    !!current_api_session.person
  end

  def current_user
    current_api_session.person
  end

  def api_session_authenticate!
    return _not_authorized unless _authorization_header &&
        current_api_session.valid?
  end

  def current_api_session
    @current_api_session ||= Session.find_by(token: _authorization_header)
  end

  def _authorization_header
    request.headers['HTTP_AUTHORIZATION']
  end

  def _not_authorized(message = 'Not Authorized')
    render json: { error: message }, status: 401
  end
end
