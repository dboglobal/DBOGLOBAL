CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6001;
	title = 600102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;";

			CDboTSActItem
			{
				iidx0 = 99077;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;";
			nolnk = 253;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99077;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 600108;
			gtype = 3;
			oklnk = 2;
			area = 600101;
			goal = 600104;
			sort = 600105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 600102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 600114;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 600101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99077;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 600109;
				ctype = 1;
				idx = -1;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 600108;
				gtype = 3;
				area = 600101;
				goal = 600104;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 600105;
				stype = 8;
				taid = 1;
				title = 600102;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "6;5;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 600107;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "6002;";
				not = 0;
			}
		}
	}
}

