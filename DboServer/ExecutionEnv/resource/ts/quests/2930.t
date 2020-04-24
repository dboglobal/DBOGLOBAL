CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2930;
	title = 293002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 293014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 293001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4501202;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 293009;
				ctype = 1;
				idx = 4511801;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 293008;
				area = 293001;
				goal = 293004;
				m0fz = "-3373.469971";
				m0widx = 1;
				scitem = -1;
				sort = 293005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2919.750000";
				m0ttip = 293015;
				rwd = 100;
				taid = 1;
				title = 293002;
				gtype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 293008;
			gtype = 1;
			oklnk = 2;
			area = 293001;
			goal = 293004;
			sort = 293005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 293002;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 293007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4511801;";
			}
		}
	}
}

