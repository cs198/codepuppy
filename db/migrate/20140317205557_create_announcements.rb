class CreateAnnouncements < ActiveRecord::Migration
  def change
    create_table :announcements do |t|
      t.references :course, index: true
      t.text :subject
      t.text :body

      t.timestamps
    end
  end
end
