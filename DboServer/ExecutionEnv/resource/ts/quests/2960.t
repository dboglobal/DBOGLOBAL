CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2960;
	title = 296002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 296007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4752110;";
			}
			CDboTSCheckClrQst
			{
				and = "2941;2953;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 296009;
				ctype = 1;
				idx = 4752110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 296008;
				area = 296001;
				goal = 296004;
				m0fz = "-812.650024";
				m0widx = 6;
				scitem = -1;
				sort = 296005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "711.950012";
				m0ttip = 296015;
				rwd = 100;
				taid = 1;
				title = 296002;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 296014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 296001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511901;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 296008;
			gtype = 1;
			oklnk = 2;
			area = 296001;
			goal = 296004;
			sort = 296005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 296002;
		}
	}
}

