CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2989;
	title = 298902;

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
			stdiag = 298907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "3041201;";
			}
			CDboTSCheckClrQst
			{
				and = "2985;";
				flink = 0;
				flinknextqid = "2990;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 298914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 298901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651212;";
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
			cont = 298908;
			gtype = 1;
			oklnk = 2;
			area = 298901;
			goal = 298904;
			sort = 298905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 298902;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 298909;
				ctype = 1;
				idx = 3041201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 298908;
				area = 298901;
				goal = 298904;
				m0fz = "-4006.929932";
				m0widx = 1;
				scitem = -1;
				sort = 298905;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4173.990234";
				m0ttip = 298915;
				rwd = 100;
				taid = 1;
				title = 298902;
				gtype = 1;
			}
		}
	}
}

