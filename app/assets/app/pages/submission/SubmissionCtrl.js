angular.module('codepuppy').controller('SubmissionCtrl', function($scope, $routeParams, $modal) {
    $scope.message = "I'm a submission page for the course " + $routeParams.courseID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
    $scope.message+= " and submission " + $routeParams.submissionID;

    $scope.comments = [];

    $scope.submissionClicked = function() {
		$scope.commentBody = {};
		$scope.commentBody.comment = "Hello!";
		var commentModal = $modal.open({
		templateUrl: '/assets/partials/commentModal/commentModal.html',
		controller: CommentModalCtrl,
		resolve: {
			commentBody: function()  {
				return $scope.commentBody;
			}
		}
		});

		commentModal.result.then(function (comment) {
			$scope.commentBody.comment = comment;
			$scope.comments.push($scope.commentBody);
		});
	};

});