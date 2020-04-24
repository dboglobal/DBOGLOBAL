CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1130;
	title = 113002;

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
				conv = 113009;
				ctype = 1;
				idx = 3142114;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 113008;
				gtype = 1;
				area = 113001;
				goal = 113004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 113005;
				stype = 1;
				taid = 1;
				title = 113002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 113014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 113001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3182103;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			cont = 113008;
			gtype = 1;
			oklnk = 2;
			area = 113001;
			goal = 113004;
			sort = 113005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 113002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 113007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSClickNPC
			{
				npcidx = "3142114;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1131;";
				not = 0;
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

