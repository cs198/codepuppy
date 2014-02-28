class Course < ActiveRecord::Base
  has_many :course_terms

  def active
    where('course_terms.active = ?', true)
  end

  validates_presence_of :course_system_id
end
