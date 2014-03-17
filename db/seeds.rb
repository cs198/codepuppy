# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)


Course.delete_all
Course.create(course_dept:'CS', course_code:'106A',
              course_name: 'Programming Methodology',
              term_name:'Fall', period:'13-14', active:false)
Course.create(course_dept:'CS', course_code:'106B',
              course_name: 'Programming Abstractions',
              term_name:'Fall', period:'13-14', active:false)

cs106a = Course.create(course_dept:'CS', course_code:'106A',
              course_name: 'Programming Methodology',
              term_name:'Winter', period:'13-14', active:true)
cs106b = Course.create(course_dept:'CS', course_code:'106B',
              course_name: 'Programming Abstractions',
              term_name:'Winter', period:'13-14', active:true)


Person.delete_all
amainero = Person.create(user_system_id:'amainero', given_name:'Anthony', family_name:'Mainero')
maesenc = Person.create(user_system_id:'maesenc', given_name:'Maesen', family_name:'Churchill')
odiab = Person.create(user_system_id:'odiab', given_name:'Omar Sebastian', family_name:'Diab')
rawatson = Person.create(user_system_id:'rawatson', given_name:'Reid', family_name:'Watson')

ActiveRecord::Base.transaction do
  CoursesPeople.delete_all
  CoursesPeople.create(course:cs106b, person:rawatson, role:'admin')
  CoursesPeople.create(course:cs106b, person:maesenc, role:'leader')
  CoursesPeople.create(course:cs106b, person:odiab, role:'student')
  CoursesPeople.create(course:cs106b, person:amainero, role:'student')

  Section.delete_all
  section = Section.create(course:cs106b)

  SectionsPeople.delete_all
  SectionsPeople.create(section:section, person:maesenc, role:'leader')
  SectionsPeople.create(section:section, person:odiab, role:'student')
  SectionsPeople.create(section:section, person:amainero, role:'student')
end

Assignment.delete_all

assignments = Array.new(7)
ActiveRecord::Base.transaction do
  description = IO.read("#{Rails.root}/db/seed_data/assignment1.txt")
  assignments[0] = Assignment.create(course:cs106b, number:1, name:'Game of Life', 
                                     date_assigned:'2014-01-10',
                                     date_due:'2014-01-17 14:00:00',
                                     description:description)

  assignments[1] = Assignment.create(course:cs106b, number:2, name:'Word Ladder / NGrams',
                                 date_assigned:'2014-01-17', 
                                 date_due:'2014-01-27 14:00:00',
                                 description: description)

  assignments[2] = Assignment.create(course:cs106b, number:3, name:'Recursion Problems',
                                 date_assigned:'2014-01-27',
                                 date_due:'2014-02-05 14:00:00',
                                 description: description)
  assignments[3] = Assignment.create(course:cs106b, number:4, name:'Boggle',
                                 date_assigned:'2014-02-05',
                                 date_due:'2014-02-14 14:00:00',
                                 description: description)
  assignments[4] = Assignment.create(course:cs106b, number:5, name:'Priority Queue',
                                 date_assigned:'2014-02-14',
                                 date_due:'2014-02-24 14:00:00',
                                 description: description)
  assignments[5] = Assignment.create(course:cs106b, number:6, name:'Huffman',
                                 date_assigned:'2014-02-24',
                                 date_due:'2014-03-05 14:00:00',
                                 description: description)
  assignments[6] = Assignment.create(course:cs106b, number:7, name:'Huffman',
                                 date_assigned:'2014-03-05',
                                 date_due:'2014-03-17 14:00:00',
                                 description: description)
end

Submission.delete_all
SubmissionFile.delete_all


first_names = ['James', 'John', 'Robert', 'Michael', 'William', 'David',
               'Richard', 'Charles', 'Thomas', 'Christopher', 'Daniel', 'Paul',
               'Mark', 'Donald', 'George', 'Kenneth', 'Steven', 'Edward',
               'Brian', 'Ronald']

last_names = ['Smith', 'Johnson', 'Williams', 'Jones', 'Brown', 'Davis',
              'Miller', 'Wilson', 'Moore', 'Taylor', 'Anderson', 'Thomas',
              'Jackson', 'White', 'Harris', 'Martin', 'Thompson']

student_objects = Hash.new
ActiveRecord::Base.transaction do
Dir.foreach("#{Rails.root}/db/seed_data") do |assignment|
  assignment_dir = "#{Rails.root}/db/seed_data/#{assignment}"
  match_results = /^assignment(\d+)$/.match(assignment)
  puts assignment
  next if match_results == nil
  assignment_number = match_results[1]
  assignment_obj = assignments[assignment_number.to_i - 1]

  Dir.foreach(assignment_dir) do |submission|
    next if submission == '.' or submission == '..'
    match_results = /(\w+)_(\d+)/.match(submission)
    student_id = match_results[1]
    if student_objects.has_key?(student_id)
      student = student_objects[student_id]
    else
      student = Person.create(user_system_id:student_id,
                              given_name:first_names.sample, 
                              family_name:last_names.sample)
      CoursesPeople.create(course:cs106b, person:student, role:'student')
      student_objects[student_id] = student
    end

    submission_dir = "#{assignment_dir}/#{submission}"
    submission_obj = Submission.create(person:student, assignment:assignment_obj,
                                       date_submitted: '2014-01-12', feedback_released:false)
    puts submission_dir
    Dir.foreach("#{submission_dir}") do |file|
      if file == 'lateDays.txt'
        latedays_contents = IO.read("#{submission_dir}/lateDays.txt")
        m = %r#student_submission_time: (\d+)/(\w+)/(\d+) ([0-9:]+)#.match(latedays_contents)
        month = Date::ABBR_MONTHNAMES.index(m[2])
        date_submitted = "#{m[3]}-%02d-%02d #{m[4]}" % [month, m[1].to_i]
        puts date_submitted
        submission_obj.date_submitted = date_submitted
        submission_obj.save
      end
      next if file == 'lateDays.txt' or file == 'GRADE.txt' or file == 'release' or file == '.' or file =='..'
      # Ignore .huf files since they can mess up the encoding
      next if file =~ /.*\.huf/
      puts "#{submission_dir}/#{file}"
      Encoding::InvalidByteSequenceError
      begin
        contents = IO.read("#{submission_dir}/#{file}")
        SubmissionFile.create(submission: submission_obj, original_filename:file, data:contents)
      rescue
      end
    end
  end
end
end
