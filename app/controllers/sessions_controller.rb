require 'hmac-sha2'
require 'securerandom'
require 'time'

class SessionsController < ApplicationController
  respond_to :json
  skip_before_filter :verify_authenticity_token
  skip_before_filter :api_session_authenticate!

  def generate
    session = Session.create(token: generate_token)
    render json: { token: session.token }
  end

  def verify
    session = _api_session

    unless session.verified ||
      verify_hmac(_authorization_header, params[:user_system_id],
                  params[:hmac])
      render_failure 'Could not validate session authenticity.'
      return
    end

    if session.expired?
      render_failure 'Session is valid, but expired.'
      return
    end

    update_verified_session(session, params[:user_system_id])
    render json: { valid: true }
  end

  private

  def update_verified_session(session, user_system_id = nil)
    person = Person.find_by user_system_id: user_system_id
    person ||= session.person

    session.person = person
    session.refresh_last_seen
    session.verified = true
    session.save
  end

  def generate_token
    SecureRandom.hex
  end

  def verify_hmac(token, user_system_id, hmac)
    # TODO: [security] move to env variable before live
    key = 'BESURETODRINKYOUROVALTINE'

    signature = "#{token},#{user_system_id}"
    hmac = HMAC::SHA256.new(key)
    hmac.update(signature)

    hmac.hexdigest == params[:hmac]
  end

  private

  def _api_session
    session = Session.find_by(token: _authorization_header)
    fail 'Session not found' if session.nil?
    session
  end

  def _authorization_header
    request.headers['HTTP_AUTHORIZATION']
  end

  def render_failure(message)
    render json: { valid: false, message: message }, status: 401
  end
end
