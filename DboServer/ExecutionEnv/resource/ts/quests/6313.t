CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 6313;
	title = 631302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 631307;
			nolnk = 3;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSClickNPC
			{
				npcidx = "4651109;";
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
				id = 974;
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
			elnk = 4;
			nextlnk = 100;
			prelnk = "2;";

			CDboTSActSendSvrEvt
			{
				id = 962;
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
		CDboTSContEnd
		{
			cid = 3;
			prelnk = "0;4;1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 631309;
				ctype = 1;
				idx = 4651109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 631308;
				gtype = 3;
				area = 631301;
				goal = 631304;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 631305;
				stype = 1;
				taid = 1;
				title = 631302;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 3;
			prelnk = "5;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 631314;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 631301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 0;

			CDboTSClickNPC
			{
				npcidx = "1251170;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 3;
			cid = 1;
			cont = 631308;
			gtype = 3;
			oklnk = 2;
			area = 631301;
			goal = 631304;
			sort = 631305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 631302;
		}
	}
}

