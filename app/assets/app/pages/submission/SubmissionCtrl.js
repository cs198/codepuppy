angular.module('codepuppy').controller('SubmissionCtrl', function($scope, $routeParams, $modal) {
    $scope.message = "I'm a submission page for the course " + $routeParams.courseID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
    $scope.message+= " and submission " + $routeParams.submissionID;

    

    $scope.submissionClicked = function() {

		var commentModal = $modal.open({
		templateUrl: '/assets/partials/commentModal/commentModal.html',
		controller: CommentModalCtrl
		});

		commentModal.result.then(function (commentBody) {
			// TODO: Create a comment directive, set its text to commentBody.
			$scope.commentBody = commentBody;
		});
	};

});
