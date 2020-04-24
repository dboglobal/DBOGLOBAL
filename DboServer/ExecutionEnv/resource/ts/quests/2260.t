CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2260;
	title = 226002;

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
				conv = 226009;
				ctype = 1;
				idx = 5101303;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 226008;
				gtype = 1;
				area = 226001;
				goal = 226004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 226005;
				stype = 1;
				taid = 1;
				title = 226002;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 226014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 226001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031103;";
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
			cont = 226008;
			gtype = 1;
			oklnk = 2;
			area = 226001;
			goal = 226004;
			sort = 226005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 226002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 226007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2261;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5101303;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

