angular.module('codepuppy').controller('LeaderSubmissionCtrl',
                                       function($scope, $routeParams, $modal, $http) {
  $scope.message = 'I\'m a submission page for the course ' +
    $routeParams.courseID;
  $scope.message += ' and assignment ' + $routeParams.assignmentID;
  $scope.message += ' and submission ' + $routeParams.submissionID;

  $scope.comments = {};

  // Map from line numbers to commentBody objects -- have .comment and .lineNumber fields
  $scope.comments = {};
  $scope.commentsReleased = false;
  $scope.commentPermissions = true;
  $scope.files = [];

  var getSubmission = function() {
    $http({method: 'GET', url: '/submissions/' + $routeParams.submissionID + '.json'})
    .success(function(data, status, headers, config) {
      $scope.commentsReleased = data.submission.feedback_released;
    });
  };

  var getFiles = function() {
    $http({method: 'GET', url: '/submissions/' + $routeParams.submissionID + '/submission_files.json'})
    .success(function(data, status, headers, config) {
      for(var i = 0; i < data.length; ++i) {
        $scope.files.push(data[i]);
      }
      if($scope.files.length > 0) {
        $scope.selectedFile = $scope.files[0];
      }
    });
  };

  getFiles();
  getSubmission();

});
