CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2998;
	title = 299802;

	CNtlTSGroup
	{
		gid = 0;

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
			stdiag = 299807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4755101;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSCheckClrQst
			{
				and = "2997;";
				flink = 0;
				flinknextqid = "2999;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 299814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 299801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511314;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 299808;
			gtype = 1;
			oklnk = 2;
			area = 299801;
			goal = 299804;
			sort = 299805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 299802;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 299809;
				ctype = 1;
				idx = 4755101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 299808;
				area = 299801;
				goal = 299804;
				m0fz = "-812.650024";
				m0widx = 6;
				scitem = -1;
				sort = 299805;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "711.950012";
				m0ttip = 299815;
				rwd = 100;
				taid = 1;
				title = 299802;
				gtype = 1;
			}
		}
	}
}

