CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1640;
	title = 164002;

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
				conv = 164009;
				ctype = 1;
				idx = 3242104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 164008;
				gtype = 1;
				area = 164001;
				goal = 164004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 164005;
				stype = 2;
				taid = 1;
				title = 164002;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 164008;
			gtype = 1;
			oklnk = 2;
			area = 164001;
			goal = 164004;
			sort = 164005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 164002;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 164014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 164001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142214;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 164007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "3242104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1641;";
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
	}
}

