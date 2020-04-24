CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3714;
	title = 371402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 371418;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 371407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 3714;
			}
			CDboTSClickObject
			{
				objidx = "11;12;13;14;15;16;17;18;19;20;";
				widx = 15;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 10;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContEnd
		{
			cid = 10;
			prelnk = "9;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1619;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 0;
				widx = 15;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 20000;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 1;
				widx = 15;
				osh_uc = 0;
				taid = 1;
			}
		}
	}
}

