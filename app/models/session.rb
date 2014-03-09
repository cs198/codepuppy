class Session < ActiveRecord::Base
  validates_presence_of :token
end
