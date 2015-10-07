#include "Ray.h"


Ray::Ray()
{
}


Ray::~Ray()
{
}

void Ray::Intersection()
{
	//for ( objects)

	//triangles 

	//for (int i = 0; i < selectedSize; i++) {
	//	index2 = selectedVertices[i];
	//	tempEdge = mVInfoArray[index2].edgePtr;

	//	do {
	//		tempE = mEdgeArray[mEdgeArray[tempEdge].nextEdge].sibling;

	//		if (mVInfoArray[mEdgeArray[tempEdge].vertex].selected != 1.0f)
	//		{
	//			linAlg::calculateVec(mVertexArray[mEdgeArray[tempEdge].vertex].xyz, mVertexArray[index2].xyz, eVec1);
	//			linAlg::calculateVec(mVertexArray[mEdgeArray[tempE].vertex].xyz, mVertexArray[index2].xyz, eVec2);
	//			linAlg::crossProd(P, newDirr, eVec2);

	//			pLength = linAlg::dotProd(eVec1, P);
	//			if (pLength < -EPSILON || pLength > EPSILON)
	//			{
	//				invP = 1.f / pLength;
	//				linAlg::calculateVec(newWPoint, mVertexArray[index2].xyz, T);

	//				u = linAlg::dotProd(T, P) * invP;
	//				if (u > 0.0f && u < 1.0f)
	//				{
	//					linAlg::crossProd(Q, T, eVec1);

	//					v = linAlg::dotProd(newDirr, Q)*invP;

	//					if (v > 0.0f && u + v < 1.0f)
	//					{
	//						t = linAlg::dotProd(eVec2, Q)*invP;
	//						if (t > EPSILON && t < 0.1f)
	//						{
	//							//sIt->next->index = e[tempEdge].triangle;
	//							tempVec[0] = newDirr[0] * t;
	//							tempVec[1] = newDirr[1] * t;
	//							tempVec[2] = newDirr[2] * t;
	//							tempE = mVInfoArray[index2].edgePtr;
	//							success = true;
	//							if (linAlg::vecLength(tempVec) < linAlg::vecLength(intersection.nxyz)){
	//								mIndex = index2;
	//								intersection.xyz[0] = newWPoint[0] + tempVec[0];
	//								intersection.xyz[1] = newWPoint[1] + tempVec[1];
	//								intersection.xyz[2] = newWPoint[2] + tempVec[2];
	//								intersection.nxyz[0] = tempVec[0];
	//								intersection.nxyz[1] = tempVec[1];
	//								intersection.nxyz[2] = tempVec[2];
	//								linAlg::crossProd(vNorm, eVec2, eVec1);
	//								linAlg::normVec(vNorm);
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//		tempEdge = tempE;

	//	} while (tempEdge != mVInfoArray[index2].edgePtr);

	//	mVInfoArray[index2].selected = 1.0f;
	//}
}