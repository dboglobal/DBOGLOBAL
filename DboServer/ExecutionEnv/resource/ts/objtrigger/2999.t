CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2999;
	title = 299902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "8;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 1;
				widx = 6;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 30000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 6;
			prelnk = "0;";

			CDboTSActCustomEvt
			{
				ceid = 299901;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "9;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 299907;
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSCheckCustomEvent
			{
				qid = 2999;
			}
			CDboTSCheckAttachObj
			{
				objidx = "51;52;53;54;55;56;";
				widx = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 8;
			prelnk = "7;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 0;
				widx = 6;
				osh_uc = 1;
				taid = 2;
			}
		}
	}
}

