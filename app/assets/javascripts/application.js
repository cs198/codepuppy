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
    .when('/:classID', {
      templateUrl : '/assets/class.html',
      controller  : 'ClassCtrl'
    })
    .when('/:classID/:assignmentID', {
      templateUrl : '/assets/assignment.html',
      controller  : 'AssignmentCtrl'
    })
    .when('/:classID/:assignmentID/:submissionID', {
      templateUrl : '/assets/submission.html',
      controller  : 'SubmissionCtrl'
    })

//} else if ($user == 'section leader') {
//     more stuff
//} else if ($user == 'lecturer') {
//     more stuff

  }
});

