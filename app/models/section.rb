class Section < ActiveRecord::Base
  has_and_belongs_to_many :people
  belongs_to :course_term
end
