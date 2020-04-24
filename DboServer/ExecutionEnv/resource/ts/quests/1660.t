CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1660;
	title = 166002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 166009;
				ctype = 1;
				idx = 6313103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 166008;
				gtype = 1;
				area = 166001;
				goal = 166004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 166005;
				stype = 1;
				taid = 1;
				title = 166002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 166007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "6313103;";
			}
			CDboTSCheckClrQst
			{
				and = "1657;";
				flink = 1;
				flinknextqid = "1661;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 166008;
			gtype = 1;
			oklnk = 2;
			area = 166001;
			goal = 166004;
			sort = 166005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 166002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 166014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 166001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242201;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

