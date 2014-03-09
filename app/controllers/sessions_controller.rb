require 'hmac-sha2'
require 'securerandom'
require 'time'

class SessionsController < ApplicationController
  respond_to :json
  skip_before_filter :verify_authenticity_token

  def generate
    session = Session.create(token: generate_token)
    render json: { token: session.token }
  end

  def verify
    unless verify_hmac(params[:token],
                       params[:user_system_id],
                       params[:hmac])
      render json: { valid: false }
      return
    end

    render json: { valid: verify_session(params[:token]) }
  end

  private

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

  def verify_session(token)
    session = Session.find_by(token: token)
    return false if session.nil?

    valid = false
    valid = true if session.created_at > 2.hours.ago
    session.destroy
    valid
  end
end
