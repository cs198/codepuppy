class Submission < ActiveRecord::Base
  has_many :comments
  belongs_to :assignment
  belongs_to :person

  validates_presence_of :student_id
  validates_presence_of :assignment_id
  validates_presence_of :location
  validates_presence_of :feedback_released
end