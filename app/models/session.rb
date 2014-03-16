class Session < ActiveRecord::Base
  belongs_to :person
  validates_presence_of :token
  before_create :init_last_seen

  TTL_TO_VERIFY = 20.minutes
  TTL_VALID = 1.month

  def expired?
    if verified
      return last_seen_at < TTL_VALID.ago
    else
      return last_seen_at < TTL_TO_VERIFY.ago
    end
  end

  def active?
    verified && !self.expired?
  end

  def refresh_last_seen
    self.last_seen_at = Time.now unless self.expired?
  end

  private

  def init_last_seen
    self.last_seen_at = Time.now
  end
end
