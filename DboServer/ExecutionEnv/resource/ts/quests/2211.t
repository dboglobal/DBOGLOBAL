CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2211;
	title = 221102;

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
				conv = 221109;
				ctype = 1;
				idx = 4651105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 221108;
				gtype = 1;
				area = 221101;
				goal = 221104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 221105;
				stype = 2;
				taid = 1;
				title = 221102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 221114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 221101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031407;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 221107;
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
				npcidx = "4651105;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2212;";
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
			cont = 221108;
			gtype = 1;
			oklnk = 2;
			area = 221101;
			goal = 221104;
			sort = 221105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 221102;
		}
	}
}
