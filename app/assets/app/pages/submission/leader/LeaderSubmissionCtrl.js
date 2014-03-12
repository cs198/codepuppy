angular.module('codepuppy').controller('LeaderSubmissionCtrl',
                                       function($scope, $routeParams, $modal) {
  $scope.message = 'I\'m a submission page for the course ' +
    $routeParams.courseID;
  $scope.message += ' and assignment ' + $routeParams.assignmentID;
  $scope.message += ' and submission ' + $routeParams.submissionID;

  $scope.comments = {};

    $scope.submissionClicked = function(index) {

		$scope.commentBody = {};
		if($scope.comments[index] !== undefined) {
			$scope.commentBody = $scope.comments[index];
		} else {
			$scope.commentBody.lineNumber = index;
			$scope.commentBody.comment = "Hello!";
		}

		var commentModal = $modal.open({
		templateUrl: '/assets/partials/commentModal/commentModal.html',
		controller: CommentModalCtrl,
		resolve: {
			commentBody: function()  {
				return $scope.commentBody;
			}
		}
		});

    commentModal.result.then(function(comment) {
      $scope.commentBody.comment = comment;
      // Used to be push
      console.log($scope.commentBody);
      $scope.comments[$scope.commentBody.lineNumber] = $scope.commentBody;
    });
  };

  // TODO: Change to an API call
  $scope.codeLines = [];
  $.get('/assets/pages/submission/code.java', function(data) {
    $scope.codeLines = data.split('\n');
  });
});
