class CreateSubmission < ActiveRecord::Migration
  def change
    create_table :submissions do |t|
      t.string :location
      t.timestamp :date_submitted
      t.boolean :feedback_released
    end

    add_reference :submissions, :student
    add_reference :submissions, :assignment
    add_index :submissions, [:student_id, :assignment_id]
  end
end
