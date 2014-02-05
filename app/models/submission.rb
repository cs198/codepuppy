class Submission < ActiveRecord::Base
  has_many :comments
  belongs_to :assignment
  belongs_to :person
end
