class CreateCourseTerm < ActiveRecord::Migration
  def change
    create_table :course_terms do |t|
      t.belongs_to :course, index: true
      t.string :course_name
      t.string :term_name
      t.string :period
      t.boolean :active
    end
  end
end
