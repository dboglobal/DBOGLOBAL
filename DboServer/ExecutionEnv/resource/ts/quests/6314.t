CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 6314;
	title = 631402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 631407;
			nolnk = 250;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSClickNPC
			{
				npcidx = "1251171;";
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 631420;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSendSvrEvt
			{
				id = 972;
				tblidx = 34012;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 631409;
				ctype = 1;
				idx = 1251171;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 631408;
				gtype = 3;
				area = 631401;
				goal = 631404;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 631405;
				stype = 1;
				taid = 1;
				title = 631402;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 5;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActSendSvrEvt
			{
				id = 964;
				tblidx = 34012;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 250;
			prelnk = "4;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 971;
			}
		}
		CDboTSContEnd
		{
			cid = 250;
			prelnk = "5;1;0;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 631414;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 631401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 0;

			CDboTSClickNPC
			{
				npcidx = "4651110;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 250;
			cid = 1;
			cont = 631408;
			gtype = 3;
			oklnk = 2;
			area = 631401;
			goal = 631404;
			sort = 631405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 631402;
		}
	}
}

