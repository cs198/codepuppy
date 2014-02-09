//= require_self
//= require_tree .

// TODO: this needs to be called after code is displayed
SyntaxHighlighter.all();

codepuppy = angular.module('codepuppy', ['ngRoute']);

codepuppy.config(function($routeProvider) {
  //if ($user == 'student') {
  if (true) {
    $routeProvider
    .when('/', {
      templateUrl : '/assets/home.html',
      controller  : 'MainCtrl'
    })
    .when('/:courseID', {
      templateUrl : '/assets/course.html',
      controller  : 'CourseCtrl'
    })
    .when('/:courseID/:assignmentID', {
      templateUrl : '/assets/assignment.html',
      controller  : 'AssignmentCtrl'
    })
    .when('/:courseID/:assignmentID/:submissionID', {
      templateUrl : '/assets/submission.html',
      controller  : 'SubmissionCtrl'
    })

//} else if ($user == 'section leader') {
//     more stuff
//} else if ($user == 'lecturer') {
//     more stuff

  }
});

