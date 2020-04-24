CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2186;
	title = 218602;

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
				conv = 218609;
				ctype = 1;
				idx = 3143123;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 218608;
				gtype = 1;
				area = 218601;
				goal = 218604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 218605;
				stype = 2;
				taid = 1;
				title = 218602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 218607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "3143123;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2187;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 218608;
			gtype = 1;
			oklnk = 2;
			area = 218601;
			goal = 218604;
			sort = 218605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 218602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 218614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 218601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3614103;";
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

