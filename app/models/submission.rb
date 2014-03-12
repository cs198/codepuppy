class Submission < ActiveRecord::Base
  has_many :comments
  belongs_to :assignment
  belongs_to :person

  validates_presence_of :person_id
  validates_presence_of :assignment_id
  validates_presence_of :date_submitted
  validates_inclusion_of :feedback_released, :in => [true, false]
end
