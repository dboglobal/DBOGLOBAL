CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2978;
	title = 297802;

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
			stdiag = 297807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickObject
			{
				objidx = "1923;";
				widx = 1;
			}
			CDboTSCheckClrQst
			{
				and = "2967;";
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
				conv = 297809;
				ctype = 1;
				idx = 1923;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 297808;
				area = 297801;
				goal = 297804;
				m0fz = "-812.650024";
				m0widx = 6;
				scitem = -1;
				sort = 297805;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "711.950012";
				m0ttip = 297815;
				rwd = 100;
				taid = 1;
				title = 297802;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 297814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 297801;
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
			cont = 297808;
			gtype = 1;
			oklnk = 2;
			area = 297801;
			goal = 297804;
			sort = 297805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 297802;
		}
	}
}

