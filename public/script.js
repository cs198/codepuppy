// create the module and name it scotchApp
var scotchApp = angular.module('scotchApp', ['ngRoute']);

// configure our routes
scotchApp.config(function($routeProvider) {
  //if ($user == 'student') {
  if (true) {
    $routeProvider
    .when('/', {
      templateUrl : 'pages/home.html',
      controller  : 'mainController'
    })
    .when('/:classID', {
      templateUrl : 'pages/class.html',
      controller  : 'classController'
    })
    .when('/:classID/:assignmentID', {
      templateUrl : 'pages/assignment.html',
      controller  : 'assignmentController'
    })

//} else if ($user == 'section leader') {
//     more stuff
//} else if ($user == 'lecturer') {
//     more stuff

  }
});


var FAKEclasses = ['cs106a', 'cs106b', 'cs106x'];
scotchApp.controller('rootController', function($scope) {
    $scope.classes = FAKEclasses;
});

// create the controller and inject Angular's $scope
scotchApp.controller('mainController', function($scope) {
    // create a message to display in our view
    $scope.message = 'Everyone come and see how good I look!';
    $scope.$parent.classes = ['cs106a', 'cs106b', 'cs106x'];
    $scope.$parent.assignments = null;
});

scotchApp.controller('classController', function($scope, $routeParams) {
    $scope.message = 'Look! I am a class page for ' + $routeParams.classID;
    $scope.$parent.classes = [$routeParams.classID];
    var FAKEassignmentsForGivenClass = ['One', 'Two']
    $scope.$parent.assignments = FAKEassignmentsForGivenClass;
    });

scotchApp.controller('assignmentController', function($scope, $routeParams) {
    $scope.$parent.classes = [$routeParams.classID];
    $scope.$parent.assignments = [$routeParams.assignmentID];
    $scope.message = "I'm an assignment page for the class " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
    });
