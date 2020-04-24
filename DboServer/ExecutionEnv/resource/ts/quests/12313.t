CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 1;
	tid = 12313;
	title = 1231302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = 0;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
			CDboTSActSendSvrEvt
			{
				id = 23260;
				tblidx = 12313;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 23280;
				tblidx = -1;
				stype = 2;
				taid = 3;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 23310;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;";

			CDboTSActSendSvrEvt
			{
				id = 23250;
				tblidx = 12313;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 4;
			nextlnk = 5;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 12313;
				start = 0;
				taid = 2;
				uof = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92012;
				eitype = 5;
				idx0 = 1234;
				cnt0 = 1;
				ectype = 0;
				etype = 0;
				qtidx = 1101104;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActWorldPlayScript
			{
				sid = 12313;
				start = 1;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1231307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 12314;
			}
			CDboTSClickObject
			{
				widx = 197000;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1101104;
			gtype = 0;
			oklnk = 2;
			area = 1101104;
			goal = 1101104;
			sort = 1101104;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 255;
			title = 1101104;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "4;2;1;0;";
			type = 0;
		}
	}
}

