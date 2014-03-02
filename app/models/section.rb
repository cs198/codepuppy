class Section < ActiveRecord::Base
  belongs_to :course
  has_many :sections_people
  has_many :people, through: :sections_people do
    def students
      where('sections_people.role = ?', 'student')
    end
    def leaders
      where('sections_people.role = ?', 'leader')
    end
  end

  validates_presence_of :course_id
  validates_presence_of :leader_id
end
