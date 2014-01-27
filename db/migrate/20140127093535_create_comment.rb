class CreateComment < ActiveRecord::Migration
  def change
    create_table :comments do |t|
      t.string :file_location
      t.integer :line_number
      t.string :comment
    end

    add_reference :comments, :submission_id, index: true
    add_index :comments, :file_location
  end
end
