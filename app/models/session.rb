class Session < ActiveRecord::Base
  belongs_to :person
  validates_presence_of :token
  before_create :init_last_seen

  TTL = 20.minutes

  def expired?
    last_seen_at < TTL.ago
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
