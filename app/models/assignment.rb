class Assignment < ActiveRecord::Base
  has_many :submissions
  belongs_to :course_term

  validates_presence_of :course_term_id
  validates_presence_of :number
  validates_presence_of :date_assigned
  validates_presence_of :date_due
end
