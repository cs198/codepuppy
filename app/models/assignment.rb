class Assignment < ActiveRecord::Base
  has_many :submissions
  belongs_to :course_term
end
